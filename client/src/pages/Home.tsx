import { useMessages, useCreateMessage } from "@/hooks/use-messages";
import { StatusCard, EmptyStatusCard } from "@/components/StatusCard";
import { Button } from "@/components/ui/button";
import { Loader2, Terminal, Plus, ArrowRight } from "lucide-react";
import { motion } from "framer-motion";

export default function Home() {
  const { data: messages, isLoading } = useMessages();
  const createMessage = useCreateMessage();

  const handleTestConnection = () => {
    createMessage.mutate({
      content: `System check ${new Date().toLocaleTimeString()} - OK`,
    });
  };

  return (
    <div className="min-h-screen bg-neutral-50 flex items-center justify-center p-6">
      <div className="w-full max-w-4xl grid grid-cols-1 lg:grid-cols-2 gap-12 items-center">
        
        {/* Left Column: Hero Content */}
        <motion.div 
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.6 }}
          className="space-y-8"
        >
          <div className="space-y-4">
            <div className="inline-flex items-center gap-2 px-3 py-1 rounded-full bg-white border shadow-sm text-xs font-medium text-muted-foreground">
              <span className="relative flex h-2 w-2">
                <span className="animate-ping absolute inline-flex h-full w-full rounded-full bg-green-400 opacity-75"></span>
                <span className="relative inline-flex rounded-full h-2 w-2 bg-green-500"></span>
              </span>
              System Operational
            </div>
            
            <h1 className="text-5xl font-semibold tracking-tight text-primary">
              Project <br />
              Initialized.
            </h1>
            
            <p className="text-lg text-muted-foreground leading-relaxed max-w-md">
              Your fullstack environment is ready. Analyze packages, build features, and scale with confidence.
            </p>
          </div>

          <div className="flex gap-4">
            <Button 
              size="lg" 
              onClick={handleTestConnection}
              disabled={createMessage.isPending}
              className="group"
            >
              {createMessage.isPending ? (
                <Loader2 className="mr-2 h-4 w-4 animate-spin" />
              ) : (
                <Terminal className="mr-2 h-4 w-4" />
              )}
              Run Diagnostics
              <ArrowRight className="ml-2 h-4 w-4 transition-transform group-hover:translate-x-1" />
            </Button>
            
            <Button variant="outline" size="lg">
              Documentation
            </Button>
          </div>
        </motion.div>

        {/* Right Column: Status Panel */}
        <motion.div 
          initial={{ opacity: 0, scale: 0.95 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ duration: 0.6, delay: 0.2 }}
          className="bg-white rounded-2xl shadow-xl border border-border/50 p-6 h-[500px] flex flex-col"
        >
          <div className="flex items-center justify-between mb-6 pb-4 border-b">
            <div className="flex items-center gap-2">
              <div className="h-3 w-3 rounded-full bg-red-500" />
              <div className="h-3 w-3 rounded-full bg-yellow-500" />
              <div className="h-3 w-3 rounded-full bg-green-500" />
            </div>
            <span className="text-xs font-mono text-muted-foreground">status_log.txt</span>
          </div>

          <div className="flex-1 overflow-y-auto space-y-3 pr-2 custom-scrollbar">
            {isLoading ? (
              <div className="flex h-full items-center justify-center text-muted-foreground">
                <Loader2 className="h-6 w-6 animate-spin" />
              </div>
            ) : messages && messages.length > 0 ? (
              messages.map((msg, idx) => (
                <StatusCard key={msg.id} message={msg.content} index={idx} />
              ))
            ) : (
              <div className="h-full flex items-center justify-center">
                <EmptyStatusCard />
              </div>
            )}
          </div>
          
          <div className="mt-4 pt-4 border-t flex items-center gap-2 text-xs text-muted-foreground font-mono">
            <span>Server Status:</span>
            <span className="text-green-600 font-bold">ONLINE</span>
            <span className="mx-2">|</span>
            <span>Latency:</span>
            <span className="text-foreground">24ms</span>
          </div>
        </motion.div>

      </div>
    </div>
  );
}
